#include <cstdio>
#include <curl/curl.h>
#include <iostream>
#include <string>

using std::string;

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  return size * nmemb;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Please enter a page url\n";
    return 1;
  }
  string pageurl = argv[1];

  CURL *curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, pageurl.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  }
  CURLcode result = curl_easy_perform(curl);
  long response_code;
  double total;
  result = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
  if (result != CURLE_OK) {
    std::cerr << curl_easy_strerror(result) << '\n';
    return 1;
  } else {
    std::cout << "Status: " << response_code << '\n';
  }
  if (result != CURLE_OK) {
    std::cerr << curl_easy_strerror(result) << '\n';
    return 1;
  } else {
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code >= 400) {
      std::cout << "HTTP error: " << response_code << '\n';
      return 1;
    }
  }
  result = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total);
  if (result != CURLE_OK) {
    std::cerr << curl_easy_strerror(result) << '\n';
    return 1;
  } else {
    double time = total * 1000;
    std::cout << "Time: " << time << '\n';
  }
  char *ct = NULL;
  result = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
  if (result != CURLE_OK) {
    std::cerr << curl_easy_strerror(result) << '\n';
    return 1;
  } else {
    std::cout << "Content-type: " << ct << '\n';
  }
  curl_easy_cleanup(curl);
}
