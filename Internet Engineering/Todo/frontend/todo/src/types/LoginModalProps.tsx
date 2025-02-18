import { UseFormReturnType } from "@mantine/form";
import { LoginFormValues } from "./LoginFormValues";

export type LoginModalProps = {
    show: boolean;
    handleClose: () => void;
    form: UseFormReturnType<LoginFormValues>
}