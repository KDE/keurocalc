# !/bin/bash
# This program shows how curconvd answers to d-bus requests

SERVICE=$(qdbus | grep curconvd)

qdbus $SERVICE /CurrencyConverter
echo

qdbus $SERVICE /CurrencyConverter DataSources
echo
qdbus $SERVICE /CurrencyConverter RoundingMethods
echo

qdbus $SERVICE /CurrencyConverter ReferenceCurrency    "(fixed)"
TABLE=$(qdbus $SERVICE /CurrencyConverter LoadSource           "(fixed)" "none")
echo $TABLE
sleep 2
qdbus $SERVICE $TABLE             AvailableCurrencies
echo
qdbus $SERVICE $TABLE             Symbol               FRF
qdbus $SERVICE $TABLE             Name                 FRF
echo "10 euro in French franc"
qdbus $SERVICE $TABLE             ConvertFromReference FRF 10.0
echo "10 French franc in euro"
qdbus $SERVICE $TABLE             ConvertToReference   FRF 10.0
qdbus $SERVICE $TABLE             Unload

qdbus $SERVICE /CurrencyConverter ReferenceCurrency    "http://www.ecb.int"
TABLE=$(qdbus $SERVICE /CurrencyConverter LoadSource           "http://www.ecb.int" "official rules")
echo $TABLE
sleep 2
qdbus $SERVICE $TABLE             AvailableCurrencies
echo
qdbus $SERVICE $TABLE             Symbol               RUB
qdbus $SERVICE $TABLE             Name                 RUB
echo "10 euro in Russian rouble"
qdbus $SERVICE $TABLE             ConvertFromReference RUB 10.0
echo "10 Russian rouble in euro"
qdbus $SERVICE $TABLE             ConvertToReference   RUB 10.0
qdbus $SERVICE $TABLE             Unload

qdbus $SERVICE /CurrencyConverter ReferenceCurrency    "http://rss.timegenie.com"
TABLE=$(qdbus $SERVICE /CurrencyConverter LoadSource           "http://rss.timegenie.com" "smallest coin")
echo $TABLE
sleep 2
qdbus $SERVICE $TABLE             AvailableCurrencies
echo
qdbus $SERVICE $TABLE             Symbol               CAD
qdbus $SERVICE $TABLE             Name                 CAD
echo "10 US dollars in Canadian dollars"
qdbus $SERVICE $TABLE             ConvertFromReference CAD 10.0
echo "10 Canadian dollars in US dollars"
qdbus $SERVICE $TABLE             ConvertToReference   CAD 10.0
qdbus $SERVICE $TABLE             Unload
