using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Project_AP;
using System.Text.Json.Serialization;

namespace Project_AP
{
    public class StockService
    {
        private readonly HttpClient httpClient;
        private readonly string apiUrl;
        private readonly string authorizationToken;

        public StockService(string apiUrl, string authorizationToken)
        {
            httpClient = new HttpClient();
            this.apiUrl = apiUrl;
            this.authorizationToken = authorizationToken;
        }
        // Получение данных о Stock через id hardware
        public async Task<List<Stock>> GetStockInfoUsingHardwareIdApi(int hardware_id)
        {
            httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", Convert.ToBase64String(System.Text.Encoding.ASCII.GetBytes(authorizationToken)));
            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

            // проверка на успешный запрос апи
            if (response.IsSuccessStatusCode)
            {
                string responseBody = await response.Content.ReadAsStringAsync();
                List <Stock> allStocks = ParseResponseStock.InfoStock(responseBody);

                // фильтры
                List<Stock> filteredStock = allStocks.FindAll(stock => (stock.hardware == hardware_id));
                return filteredStock;
            }
            else
            {
                throw new Exception($"API request failed with status code: {response.StatusCode}");
            }
        }
        // Получение данных о Stock через его id
        public async Task<Stock> GetStockByIdApi(int stock_id)
        {
            httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", Convert.ToBase64String(System.Text.Encoding.ASCII.GetBytes(authorizationToken)));
            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

            // Проверка успешного запроса к апи
            if (response.IsSuccessStatusCode)
            {
                string responseBody = await response.Content.ReadAsStringAsync();
                List<Stock> allStocks = ParseResponseStock.InfoStock(responseBody);
                // Фильтры
                Stock stock = allStocks.SingleOrDefault(stock => (stock.id == stock_id));
                return stock;
            }
            else
            {
                throw new Exception($"API request failed with status code: {response.StatusCode}");
            }
        }
        // Получение данных о stock через id rack
        public async Task<List<Stock>> GetStockInfoUsingRackIdApi(int rack_id)
        {
            httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", Convert.ToBase64String(System.Text.Encoding.ASCII.GetBytes(authorizationToken)));
            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

            // Проверка на успешный запрос к апи
            if (response.IsSuccessStatusCode)
            {
                string responseBody = await response.Content.ReadAsStringAsync();
                List<Stock> allStocks = ParseResponseStock.InfoStock(responseBody);

                // Фильтры
                List<Stock> filteredStock = allStocks.FindAll(stock => (stock.rack == rack_id));
                return filteredStock;
            }
            else
            {
                throw new Exception($"API request failed with status code: {response.StatusCode}");
            }
        }
        // Создать новый stock и вернуть его id
        public async void CreateNewStockApi(Stock stock) //Task<int>
        {
            List<Stock> st = new();
            st.Add(stock);
            httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", Convert.ToBase64String(System.Text.Encoding.ASCII.GetBytes(authorizationToken)));
            string jsonData = Newtonsoft.Json.JsonConvert.SerializeObject(st, Formatting.Indented);
            var content = new StringContent(jsonData, Encoding.UTF8, "application/json");

            HttpResponseMessage response = await httpClient.PostAsync(apiUrl, content);
            if (response.IsSuccessStatusCode)
            {
                string responseBody = await response.Content.ReadAsStringAsync();
                // return ParseResponseStock.OnlyIdStock(responseBody);
            }
            else
            {
                throw new Exception($"API request failed with status code: {response.StatusCode}");
            }
        }
        // Удалить stock по его id
        public async void DeleteStockApi(int stock_id)
        {
            string url_del = apiUrl + "/" + stock_id;
            httpClient.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Basic", Convert.ToBase64String(System.Text.Encoding.ASCII.GetBytes(authorizationToken)));
            HttpResponseMessage response = await httpClient.DeleteAsync(url_del);

            // Проверка статуса ответа
            if (!response.IsSuccessStatusCode)
            {
                throw new Exception($"API request failed with status code: {response.StatusCode}");
            }
        }
    }

    // Описание класса
    public class Stock
    {
        [JsonPropertyName("hardware")]
        public int hardware { get; set; }
        [JsonPropertyName("rack")]
        public int rack { get; set; }
        [JsonPropertyName("rack_position")]
        public int rack_position { get; set; }
        public int id { get; set; }
        [JsonPropertyName("count")]
        public int count { get; set; }
    }
}

