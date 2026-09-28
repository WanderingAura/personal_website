package views

import (
	"embed"
	"html/template"
	"net/http"
)

//go:embed layout.html partials/*.html
var templateFS embed.FS

// PageData is the top-level context passed to layout.html. Data holds
// whatever a specific route's content partial needs.
type PageData struct {
	Title     string
	ActiveNav string
	Data      any
}

var routeTemplates = map[string]*template.Template{}

func init() {
	for _, partial := range []string{
		"home.html",
		"about.html",
		"projects.html",
		"contact_form.html",
		"contact_success.html",
	} {
		tmpl, err := template.ParseFS(templateFS, "layout.html", "partials/nav.html", "partials/"+partial)
		if err != nil {
			panic("views: failed to parse " + partial + ": " + err.Error())
		}
		routeTemplates[partial] = tmpl
	}
}

// RenderPage renders contentPartial (a file under views/partials/) as a bare
// fragment for htmx swaps (HX-Request header present) or wrapped in the full
// page layout for direct navigation and hard refreshes.
func RenderPage(w http.ResponseWriter, r *http.Request, contentPartial string, page PageData) {
	tmpl, ok := routeTemplates[contentPartial]
	if !ok {
		http.Error(w, "views: unknown template "+contentPartial, http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "text/html; charset=utf-8")

	if r.Header.Get("HX-Request") == "true" {
		if err := tmpl.ExecuteTemplate(w, "content", page.Data); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
		}
		return
	}

	if err := tmpl.ExecuteTemplate(w, "layout.html", page); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

// RenderFragment renders a single named template from contentPartial's
// template set directly, bypassing both the "content" block and the layout.
// Used for htmx form responses that only ever swap one inner element (e.g.
// the contact form) regardless of full-page vs. partial navigation.
func RenderFragment(w http.ResponseWriter, contentPartial, fragmentName string, data any) {
	tmpl, ok := routeTemplates[contentPartial]
	if !ok {
		http.Error(w, "views: unknown template "+contentPartial, http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "text/html; charset=utf-8")

	if err := tmpl.ExecuteTemplate(w, fragmentName, data); err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}
