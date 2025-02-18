import { Modal } from "react-bootstrap"
import { LoginForm } from "./LoginForm"
import { FC } from "react";
import { LoginModalProps } from "../../types/LoginModalProps";

export const LoginModal: FC<LoginModalProps> = ({ show, handleClose, form }) => {
    return (
        <Modal size="lg" aria-labelledby="" centered show={show} onHide={handleClose}>
            <Modal.Header closeButton>
                <Modal.Title id="login-modal-title">
                    Witaj!
                </Modal.Title>
            </Modal.Header>
            <Modal.Body>
                <LoginForm form={form} handleClose={handleClose}/>
            </Modal.Body>
        </Modal>
    )
}