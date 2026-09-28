package handlers

import (
	"net/http"
	"net/mail"
	"strings"

	pmail "webserver/mail"
	"webserver/views"
)

type contactFormData struct {
	Name, Email, Message string
	Errors               []string
}

// Contact handles POST /contact: validate, hand off to the mailer, and
// re-render the #contact-form element in place (never the whole page or
// #content) so this works identically regardless of full-page vs. htmx nav.
func Contact(mailer pmail.Mailer) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		form := contactFormData{
			Name:    strings.TrimSpace(r.FormValue("name")),
			Email:   strings.TrimSpace(r.FormValue("email")),
			Message: strings.TrimSpace(r.FormValue("message")),
		}

		if form.Name == "" {
			form.Errors = append(form.Errors, "Name is required.")
		}
		if form.Message == "" {
			form.Errors = append(form.Errors, "Message is required.")
		}
		if _, err := mail.ParseAddress(form.Email); err != nil {
			form.Errors = append(form.Errors, "Please enter a valid email address.")
		}

		if len(form.Errors) > 0 {
			views.RenderFragment(w, "contact_form.html", "contact-form-fragment", form)
			return
		}

		sub := pmail.ContactSubmission{Name: form.Name, Email: form.Email, Message: form.Message}
		if err := mailer.Send(r.Context(), sub); err != nil {
			form.Errors = []string{"Something went wrong sending your message — please try again."}
			views.RenderFragment(w, "contact_form.html", "contact-form-fragment", form)
			return
		}

		views.RenderFragment(w, "contact_success.html", "contact-success-fragment", nil)
	}
}
