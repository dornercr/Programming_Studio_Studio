set -eu
openssl req -x509 -newkey rsa:2048 -nodes -keyout key.pem -out cert.pem -days 1 -subj /CN=worker.test -addext subjectAltName=DNS:worker.test 2>certificate.log
g++ -std=c++20 check.cpp -lssl -lcrypto -o check
./check
