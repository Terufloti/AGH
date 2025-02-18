import { UseFormReturnType } from "@mantine/form";
import { LoginFormValues } from "./LoginFormValues";

export type LoginFormProps = {
    form: UseFormReturnType<LoginFormValues>;
    handleClose: () => void;
}