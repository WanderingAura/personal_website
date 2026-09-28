package mail

import (
	"context"
	"log"
)

// NoopMailer logs contact form submissions instead of sending mail.
type NoopMailer struct{}

func (NoopMailer) Send(_ context.Context, sub ContactSubmission) error {
	log.Printf("contact form submission: name=%q email=%q message=%q", sub.Name, sub.Email, sub.Message)
	return nil
}
