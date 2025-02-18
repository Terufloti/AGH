import { API_URL } from "../../../config";
import ky from "ky";

export const updateTodo = async (id: number, currentDone:boolean ) => {
    const newDoneStatus = !currentDone;
    console.log('Aktualizacja zadania ', id, ', done: ', currentDone, ' -> ', newDoneStatus);
    return ky.patch(`${API_URL}/todo/${id}`, {json: { done: newDoneStatus }, credentials: "include"}).json();
};