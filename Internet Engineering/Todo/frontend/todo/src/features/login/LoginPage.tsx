import { Button, Col, Container, Row } from "react-bootstrap";
import { WelcomeMessage } from "./LoginWelcomeMessage";
import { useState } from "react";
import { LoginModal } from "./LoginModal";
import { useLoginForm } from "./hooks/useLoginForm";

export const LoginPage = () => {
  const [showLoginModal, setShowLoginModal] = useState(false);
  const form = useLoginForm();

  return (
    <Container
      fluid
      className="d-flex vh-100 justify-content-center align-items-center"
    >
      <Col className="text-center">
        <Row id="welcome-message-row">
          <WelcomeMessage
            text="Czy istnieje Lepsze Todo? Bez szans."
            speed={75}
            delay={5000}
          />
        </Row>
        <Row className="justify-content-center">
          <Col xs="auto">
            <Button
              variant="outline-warning"
              className="w-auto"
              onClick={() => setShowLoginModal(true)}
            >
              Zaloguj się!
            </Button>
          </Col>
        </Row>
      </Col>
      <LoginModal show={showLoginModal} handleClose={() => setShowLoginModal(false)} form={form} />
    </Container>
  );
};
