package mail

import "context"

// ContactSubmission is the validated payload from the contact form.
type ContactSubmission struct {
	Name    string
	Email   string
	Message string
}

// Mailer delivers a contact form submission. NoopMailer is wired in by
// default; a real SMTP/SendGrid/Postmark implementation can be dropped in
// later by satisfying this interface, with no handler changes required.
type Mailer interface {
	Send(ctx context.Context, sub ContactSubmission) error
}
