import { FC, useState } from "react";
import { Container, Form, Button } from "react-bootstrap";
import { LoginFormProps } from "../../types/LoginFormProps";
import { useNavigate } from "react-router-dom";
import { login } from "./api/login";
import { LoginToast } from "./LoginToast";


export const LoginForm: FC<LoginFormProps> = ({ form, handleClose }) => {
    const navigate = useNavigate();
    const [errorMessage, setErrorMessage] = useState<string | null>(null);

    const handleSubmit = async (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault();
        const validationResult = form.validate();

        if(validationResult.hasErrors) {
            setErrorMessage("Wrong data. Check your email and password.")
            return;
        }

        try {
            const { email, password } = form.values;

            await login(email, password);
            
            navigate('/todo');
            handleClose();
        } catch (error) {
            console.error("Login Error", error);
            if(error instanceof Error) {
                setErrorMessage(error.message);
            } else {
                setErrorMessage("Unknown login error occured");
            }
        }
    };

    return (
        <Container fluid>
            {errorMessage && <LoginToast message={errorMessage} />}
            <Form onSubmit={handleSubmit}>
                <Form.Group className='mb-3' controlId="formBasicLogin">
                    <Form.Label>Email</Form.Label>
                    <Form.Control
                        type="email"
                        placeholder="admin@admin.com"
                        {...form.getInputProps("email")}
                    />
                    <Form.Text className="text-muted">
                        Please write down your email.
                    </Form.Text>
                </Form.Group>
                <Form.Group className="mb-3" controlId="formBasicPassword">
                    <Form.Control
                        type="password"
                        placeholder="Password"
                        {...form.getInputProps("password")}
                    />
                    <Form.Text className="text-muted">
                        We'll never share your password :).
                    </Form.Text>
                </Form.Group>
                <Button variant="outline-success" type="submit">Zaloguj sie</Button>
            </Form>
        </Container>
    );
};