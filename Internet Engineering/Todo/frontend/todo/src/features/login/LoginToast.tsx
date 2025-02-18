import { useState } from "react"
import { Toast, ToastContainer } from "react-bootstrap";

export const LoginToast = ({ message }: { message: string }) => {
    const [show, setShow] = useState(true);

    return (
        <ToastContainer position="bottom-end" className="p-3">
            <Toast bg="danger" onClose={() => setShow(false)} show={show} delay={5000} autohide>
                <Toast.Header>
                    <i className="bi bi-bell-slash"></i><strong className="me-auto">Login failed!</strong>
                </Toast.Header>
                <Toast.Body>{message}</Toast.Body>
            </Toast>
        </ToastContainer>
    )
}