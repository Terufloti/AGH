import { useForm, UseFormReturnType } from "@mantine/form";
import { LoginFormValues } from "../../../types/LoginFormValues";

export const useLoginForm = (): UseFormReturnType<LoginFormValues> => {
  const form = useForm<LoginFormValues>({
    initialValues: {
      email: "",
      password: "",
    },
    validate: {
        email: (value) => (/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/.test(value) ? null : "Not valid email."),
        password: (value) => (value.length >= 8 ? null : "Password must be at least 8 characters long."),
    },
  });

  return form;
};
