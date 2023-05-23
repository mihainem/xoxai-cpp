#ifndef LOCALIZATIONS_H
#define LOCALIZATIONS_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Define a callback function for curl to write received data into a string
static size_t writeCallback(char *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append(ptr, size * nmemb);
    return size * nmemb;
}
const std::string &csv_url = "https://docs.google.com/spreadsheets/d/e/2PACX-1vQqT-K5Hbn74oGF1Ejdkgz_eQtho-wA098XOXlWp0NjjiCVvpAQasqJPF7NFYJdCDF1CBRolhDxBxUr/pub?gid=0&single=true&output=csv";

json get_language(const std::string &language_code)
{
    // Set up curl
    CURL *curl = curl_easy_init();
    if (!curl)
    {

        std::cerr << "Error initializing curl" << std::endl;
        return json();
    }

    // Set curl options
    curl_easy_setopt(curl, CURLOPT_URL, csv_url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    // Perform curl request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        std::cerr << "Error performing curl request: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return json();
    }

    // Clean up curl
    curl_easy_cleanup(curl);

    // Convert received CSV data into a map of language dicts
    std::istringstream csv_stream(response_string);
    std::vector<std::string> headers;
    std::string line;
    std::getline(csv_stream, line); // get the header line
    std::istringstream header_stream(line);
    std::string header;
    while (std::getline(header_stream, header, ','))
    {
        headers.push_back(header);
    }
    std::map<std::string, json> language_dicts;
    while (std::getline(csv_stream, line))
    {
        std::istringstream row_stream(line);
        std::string cell;
        std::getline(row_stream, cell, ','); // first column is index
        std::string language_code = cell;
        json language_dict;
        int col_index = 1;
        while (std::getline(row_stream, cell, ','))
        {
            language_dict[headers[col_index]] = cell;
            ++col_index;
        }
        language_dicts[language_code] = language_dict;
    }

    // Return the requested language dict
    auto iter = language_dicts.find(language_code);
    if (iter == language_dicts.end())
    {
        std::cerr << "Language code not found: " << language_code << std::endl;
        return json();
    }
    return iter->second;
}

#endif
