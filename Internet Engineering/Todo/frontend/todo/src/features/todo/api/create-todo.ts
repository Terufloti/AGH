import { TodoFormValues } from "../../../types/TodoFormValues";
import ky from "ky";
import { API_URL } from "../../../config";
import { TodoType } from "../../../types/TodoType";

export const createTodo = async(data: TodoFormValues) => {
    console.log("Wysylanie zadania: ", data);
    return ky.post(`${API_URL}/todo`, {json: data, credentials: "include"}).json<TodoType>();
}