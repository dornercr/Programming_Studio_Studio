set -eu
openssl req -x509 -newkey rsa:2048 -nodes -keyout key.pem -out cert.pem -days 1 -subj /CN=lab.test 2>generation.log
openssl x509 -in cert.pem -noout -checkend 0 >now.txt
if openssl x509 -in cert.pem -noout -checkend 172800 >later.txt; then exit 1; fi
printf 'valid now; expiration occurs within48h\n'
