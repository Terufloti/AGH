import { Button, Form, Container } from "react-bootstrap";
import { useTodoForm } from "./hooks/useTodoForm"
import { createTodo } from "./api/create-todo";
import { useNavigate } from "react-router-dom";

export const TodoForm = () => {
    const form = useTodoForm();
    const navigate = useNavigate();

    const handleSubmit = async (event: React.FormEvent<HTMLFormElement>) => {
        event.preventDefault();
        if(form.validate().hasErrors) return;
        try {
            console.log(form.values);
            await createTodo(form.values);
            navigate('/todo');
        } catch (error) {
            console.log(error);
        }
    }

    return (
        <Container fluid>
            <Form onSubmit={handleSubmit}>
            <Form.Group className='mb-3' controlId="formBasicTitle">
                <Form.Label> Title </Form.Label>
                <Form.Control 
                    type="text" 
                    placeholder="Enter task title..." 
                    {...form.getInputProps("title")}
                />
                <Form.Text className="text-muted">
                    This is your task name! Be creative...
                </Form.Text>
            </Form.Group>

            <Form.Group className="mb-3" controlId="formBasicDescription">
                <Form.Label> Description </Form.Label>
                <Form.Control 
                    as="textarea" 
                    placeholder="Descript your task..." 
                    rows={3}
                    {...form.getInputProps("content")}    
                />
                <Form.Text className="text-muted">
                    Write you task description in case you forgot!
                </Form.Text>
            </Form.Group>

            <Form.Group className="mb-3" controlId="formBasicCheckbox">
                <Form.Check 
                    type="checkbox" 
                    label="Done?" 
                    {...form.getInputProps("done", { type: "checkbox" })}
                />
            </Form.Group>

            <Button variant="outline-success" type="submit">
                <i className="bi bi-plus-circle"></i> Add new task!
            </Button>
        </Form>
        </Container>
        
    )
}
