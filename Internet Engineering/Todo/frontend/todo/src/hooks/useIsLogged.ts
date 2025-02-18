import Cookies from "universal-cookie"

export const useIsLogged = () => {
    const cookies = new Cookies();
    return cookies.get('is-logged') !== undefined;
}