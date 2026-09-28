package handlers

import (
	"net/http"

	"webserver/data"
	"webserver/views"
)

func Home(w http.ResponseWriter, r *http.Request) {
	views.RenderPage(w, r, "home.html", views.PageData{
		Title:     "Jiachen Wang",
		ActiveNav: "home",
	})
}

func About(w http.ResponseWriter, r *http.Request) {
	views.RenderPage(w, r, "about.html", views.PageData{
		Title:     "About - Jiachen Wang",
		ActiveNav: "about",
	})
}

func Projects(w http.ResponseWriter, r *http.Request) {
	views.RenderPage(w, r, "projects.html", views.PageData{
		Title:     "Projects - Jiachen Wang",
		ActiveNav: "projects",
		Data:      data.Projects,
	})
}

func ContactPage(w http.ResponseWriter, r *http.Request) {
	views.RenderPage(w, r, "contact_form.html", views.PageData{
		Title:     "Contact - Jiachen Wang",
		ActiveNav: "contact",
		Data:      contactFormData{},
	})
}
