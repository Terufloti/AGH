import { API_URL } from "../../../config"
import ky from "ky";

export const logout = async () => {
    try {
        await ky.post(`${API_URL}/auth/logout`, {credentials: "include"});
        localStorage.removeItem('access-token');
        localStorage.removeItem('is-logged');
    } catch (error) {
        console.error("Nie wylogujesz sie nigdy rozumiesz?", error);
    }
}