package data

// Project is a single entry in the portfolio's projects list.
type Project struct {
	Title       string
	Description string
	URL         string
}

// Projects is a static seed list. Swapping this for a function that loads
// from a JSON file or database later doesn't require any handler/template
// changes, since both only depend on the []Project shape.
var Projects = []Project{
	{
		Title:       "Personal Gallery",
		Description: "This site's raylib-powered gallery — a boids flocking sim, a fractal painting renderer, and a few other interactive pieces, running natively and as WebAssembly.",
	},
}
