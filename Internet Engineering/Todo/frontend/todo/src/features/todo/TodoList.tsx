import { TodoType } from "../../types/TodoType";
import { Container, Col, Row } from "react-bootstrap";
import { TodoListItem } from "./TodoListItem";
import { useEffect, useState } from "react";
import { listTodo } from "./api/todo";
import { useLocation } from "react-router-dom";

export const TodoList = () => {
    const [data, setData] = useState<TodoType[]>([]);
    const location = useLocation();

    useEffect(() => {
        listTodo().then((response) => setData(response));
    }, [location])

    return (
        <Container className="mt-4">
            <Row className="justify-content-center g-4">
                {data.map((task) => (
                    <Col key={task.id} lg={3} md={4} sm={10} className="d-flex justify-content-center">
                        <TodoListItem item={task} />
                    </Col>
                ))}
            </Row>
        </Container>
    )
}