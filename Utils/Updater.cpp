#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <shellapi.h>
#include "Updater.h"
#include "Config.h"
using json = nlohmann::json;

static string trim_string(const string& str) {
    const size_t MAX_LENGTH = 150;
    if (str.size() > MAX_LENGTH)
        return str.substr(0, MAX_LENGTH) + "...[]";
    return str;
}


static size_t curl_callback(void* contents, const size_t size, const size_t nmemb, const void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static void open_url(const string& url) {
    if (ShellExecuteW(0, L"open", Utils::string_to_wstring(url).c_str(), 0, 0, SW_SHOWNORMAL) <= (HINSTANCE)32)
        LOG::ERR("Failed to open the browser. Please visit {}", url);
}

optional<string> Updater::web_request() {
    CURL* curl;
    CURLcode res;
    string read_buffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, Updater::update_url.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dota2Patcher");
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            LOG::CRITICAL("Can't check update. CURL Error: {}", curl_easy_strerror(res));
            return nullopt;
        }
    }

    return read_buffer;
}

SemVer Updater::get_latest_version(const std::vector<WebVer>& web_versions) {
    SemVer latest_release = local_version;
    SemVer latest_RC = local_version;

    for (const auto& web_ver : web_versions) {
        SemVer parsed = SemVer::from_string(web_ver.tag_name);

        if (web_ver.prerelease) {
            if (parsed > latest_RC) {
                latest_RC = parsed;
                latest_RC.update_url = web_ver.html_url;
                latest_RC.body = web_ver.body;
            }
        }
        else {
            if (parsed > latest_release) {
                latest_release = parsed;
                latest_release.update_url = web_ver.html_url;
                latest_release.body = web_ver.body;
            }
        }
    }

    if (ConfigManager::config_entries["allow_rc_update"])
        return (latest_RC > latest_release) ? latest_RC : latest_release;

    return latest_release;
}

bool Updater::update_required() {
    LOG::INFO("Current version: {}", Updater::local_version.to_string());

#ifdef _DEBUG
    return false;
#endif

    auto web_resp = web_request();
    if (!web_resp)
        return false;

    std::vector<WebVer> web_versions;

    try {
        json parsed_json = json::parse(web_resp.value());

        for (int i = 0; i < 5; i++) {
            string tag_name = parsed_json[i]["tag_name"];
            bool prerelease = parsed_json[i]["prerelease"];
            string html_url = parsed_json[i]["html_url"];
            optional <string> body = parsed_json[i]["body"];
            if (!body || body.value().empty())
                body = nullopt;

            web_versions.push_back({ tag_name, prerelease, html_url, body });
        }
    }
    catch (const json::parse_error& e) {
        LOG::CRITICAL("(Updater) JSON parse error: {}", e.what());
        return false;
    }
    catch (const std::exception& e) {
        LOG::CRITICAL("(Updater) Error: {}", e.what());
        return false;
    }

    SemVer latest_version = Updater::get_latest_version(web_versions);
    if (Updater::local_version < latest_version) {
        LOG::ERR("Update Required! New version: {}", latest_version.to_string());
        if (latest_version.body)
            LOG::INFO("Update Log: {}", trim_string(latest_version.body.value()));

        system("pause");
        open_url(latest_version.update_url.value());

        return true;
    }

    return false;
}
