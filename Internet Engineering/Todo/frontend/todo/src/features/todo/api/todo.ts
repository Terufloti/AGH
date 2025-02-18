import ky from "ky";
import { API_URL } from "../../../config";
import { TodoType } from "../../../types/TodoType";

export const listTodo = async () => {
  return ky
    .get(`${API_URL}/todo`, { credentials: "include" })
    .json<TodoType[]>();
};
