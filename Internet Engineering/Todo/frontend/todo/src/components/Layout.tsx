import { Outlet } from "react-router-dom";
import { Navigation } from "./Navigation.tsx";

export const Layout = () => {
    return (
        <div>
            <Navigation />
            <Outlet />  
        </div>
    );
}
