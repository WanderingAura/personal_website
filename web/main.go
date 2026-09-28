package main

import (
	"log"
	"mime"
	"net/http"

	"webserver/mail"
)

func main() {
	if err := mime.AddExtensionType(".wasm", "application/wasm"); err != nil {
		log.Fatalf("failed to register wasm mime type: %v", err)
	}

	handler := newRouter(mail.NoopMailer{})

	const addr = ":8080"
	log.Printf("listening on %s", addr)
	if err := http.ListenAndServe(addr, handler); err != nil {
		log.Fatal(err)
	}
}
