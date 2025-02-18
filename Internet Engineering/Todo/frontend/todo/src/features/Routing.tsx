import { Navigate, RouteObject, useRoutes } from "react-router-dom";
import { Layout } from "../components/Layout";
import { TodoList } from "./todo/TodoList";
import { TodoForm } from "./todo/TodoForm";
import { ErrorPage } from "./error/ErrorPage";
import { LoginPage } from "./login/LoginPage";
import { useIsLogged } from "../hooks/useIsLogged";

const publicRoutes: RouteObject[] = [
    {
        path: '/',
        children: [
            {
                path: '/login',
                element: <LoginPage/>
            },
            {
                path: '*',
                element: <Navigate to="/login" replace/>
            }
        ]
    }
]

const privateRoutes: RouteObject[] = [
  {
    path: "/",
    element: <Layout />,
    children: [
      {
        path: "/todo",
        element: <TodoList />,
      },
      {
        path: "/todo/new",
        element: <TodoForm />,
      },
      {
        path: "todo/:id",
        element: <TodoForm />,
      },
      {
        path: "*",
        element: <ErrorPage />,
      },
    ],
  },
];

export const Routing = () => {
  const isLogged = useIsLogged();
  const routes = isLogged ? privateRoutes : publicRoutes;
  return useRoutes(routes);
};
