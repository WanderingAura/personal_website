package main

import (
	"net/http"

	"webserver/handlers"
	"webserver/mail"
)

func newRouter(mailer mail.Mailer) http.Handler {
	mux := http.NewServeMux()

	mux.HandleFunc("GET /", handlers.Home)
	mux.HandleFunc("GET /about", handlers.About)
	mux.HandleFunc("GET /projects", handlers.Projects)
	mux.HandleFunc("GET /contact", handlers.ContactPage)
	mux.HandleFunc("POST /contact", handlers.Contact(mailer))

	fileServer := http.FileServer(http.Dir("static"))
	mux.Handle("GET /static/", http.StripPrefix("/static/", fileServer))

	return mux
}
