import { Container, Navbar, Offcanvas, Nav, Button } from "react-bootstrap";
import { NavLink, useNavigate } from "react-router-dom";
import "bootstrap-icons/font/bootstrap-icons.css";
import { useState } from "react";
import { logout } from "../features/logout/api/auth";

export const Navigation = () => {
  const [showOffcanvas, setShowOffcanvas] = useState(false);
  const navigate = useNavigate();

  const handleLogout = async () => {
    await logout();
    navigate("/login");
  };

  return (
    <>
      <Navbar expand="xxl" className="bg-body-tertiary mb-3">
        <Container fluid>
          <Navbar.Brand>Nawigacja</Navbar.Brand>
          <Navbar.Toggle
            aria-controls="offcanvasNavbar"
            onClick={() => setShowOffcanvas(true)}
          />
          <Navbar.Offcanvas
            id="offcanvasNavbar"
            aria-labelledby="offcanvasNavbarLabel"
            show={showOffcanvas}
            onHide={() => setShowOffcanvas(false)}
          >
            <Offcanvas.Header closeButton>
              <Offcanvas.Title id="offcanvasNavbarLabel">
                Nawigacja
              </Offcanvas.Title>
            </Offcanvas.Header>
            <Offcanvas.Body>
              <Container fluid>
                <div className="d-flex flex-column flex-xxl-row gap-2 justify-content-between align-items-center">
                  <div className="d-flex flex-column flex-xxl-row gap-2 me-auto">
                    <Nav.Link as={NavLink} to="/todo">
                      <Button
                        variant="outline-secondary"
                        className={showOffcanvas ? "w-100" : "w-auto"}
                      >
                        <i className="bi bi-card-checklist"></i> TODO List
                      </Button>
                    </Nav.Link>
                    <Nav.Link as={NavLink} to="/todo/new">
                      <Button
                        variant="outline-success"
                        className={showOffcanvas ? "w-100" : "w-auto"}
                      >
                        <i className="bi bi-pencil-square"></i> New task
                      </Button>
                    </Nav.Link>
                  </div>
                  <div className="ms-auto">
                    <Button variant="outline-danger" onClick={handleLogout}>
                      Wyloguj się!
                    </Button>
                  </div>
                </div>
              </Container>
            </Offcanvas.Body>
          </Navbar.Offcanvas>
        </Container>
      </Navbar>
    </>
  );
};
