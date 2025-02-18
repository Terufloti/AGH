/* eslint-disable */
import { createParamDecorator, ExecutionContext } from "@nestjs/common";

export const UserID = createParamDecorator<number>(
    (data: unknown, ctx: ExecutionContext) => {
        const request = ctx.switchToHttp().getRequest();
        return request.userId;
    },
)