#include <iostream>
#include <string>
#include <curl/curl.h>

// cURL жауабын алуға арналған функция
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // 1. Meta Developer порталынан алған деректерді жазыңыз
    std::string phone_number_id = "YOUR_PHONE_NUMBER_ID"; // Мысалы: "1059372138..."
    std::string access_token = "YOUR_ACCESS_TOKEN";       // Уақытша немесе тұрақты токен
    std::string recipient_phone = "";          // Алушының нөмірі (ел кодымен, '+' белгісіз)

    // 2. Meta Cloud API URL мекенжайы
    std::string url = "https://graph.facebook.com/v18.0/" + phone_number_id + "/messages";

    // 3. Жіберілетін JSON дерегі
    std::string json_payload = "{"
        "\"messaging_product\": \"whatsapp\","
        "\"to\": \"" + recipient_phone + "\","
        "\"type\": \"text\","
        "\"text\": {\"body\": \"Сәлем! Бұл Windows C++ арқылы жіберілген хабарлама!\"}"
        "}";

    // 4. cURL арқылы HTTP POST сұранысын жіберу
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string response_string;
        struct curl_slist* headers = NULL;

        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        // Windows-та SSL сертификатын тексеру мәселесі болмас үшін (тестілеу кезінде):
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Қате орын алды: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Сервер жауабы:\n" << response_string << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    system("pause"); // Консоль жабылып қалмауы үшін
    return 0;
}