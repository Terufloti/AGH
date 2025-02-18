import { Card, Badge } from "react-bootstrap";
import { FC, memo, useState } from "react";
import { TodoListItemType } from "../../types/TodoListItemType";
import { NavLink, useNavigate } from "react-router-dom";
import { updateTodo } from "./api/update-todo";

export const TodoListItem: FC<TodoListItemType> = memo(({ item }) => {
  const [done, setDone] = useState(item.done);
  const [isUpdating, setIsUpdating] = useState(false);
  const navigate = useNavigate();

  const handleMarkAsDone = async () => {
    if (isUpdating) return;

    setIsUpdating(true);

    try {
      const updatedTodo = await updateTodo(item.id, done);
      setDone(updatedTodo.done);
    } catch (error) {
      console.error("Blad aktualizacji zadania: ", error);
    } finally {
      setIsUpdating(false);
    }
  };

  const handleNavigateToTodo = (event: React.MouseEvent) => {
    event.stopPropagation();
    navigate(`/todo/${item.id}`);
  };

  return (
    <Card
      style={{ width: "15rem", position: "relative" }}
      className={done ? "opacity-50" : ""}
      onClick={handleMarkAsDone}
    >
      {done && (
        <Badge
          pill
          bg="success"
          className="position-absolute top-0 end-0"
          style={{
            transform: "translate(50%, -50%)",
            padding: "4px 6px",
          }}
        >
          Done!
        </Badge>
      )}
      <Card.Img variant="top" src="https://placehold.co/400x200" />
      <Card.Body className="d-flex flex-column">
        <div className="d-flex justify-content-between align-items-center">
          <Card.Title>{item.title}</Card.Title>
        </div>
        <Card.Text>{item.content}</Card.Text>
        <div className="d-flex justify-content-between mt-3">
          <NavLink
            to={`/todo/${item.id}`}
            className="btn btn-outline-danger d-flex justify-content-center w-100"
            onClick={handleNavigateToTodo}
          >
            Dive in!
          </NavLink>
        </div>
      </Card.Body>
    </Card>
  );
});
