/* eslint-disable */
import { Transform } from "class-transformer";
import { IsEnum, IsOptional } from "class-validator";

export class TodoFilterDto {
    @IsOptional()
    @IsEnum(['createdAt', 'title'])
    sortBy?: string = 'createdAt';

    @IsOptional()
    @IsEnum(['asc', 'desc'])
    sortOrder?: 'asc' | 'desc' = 'desc';
    
    @IsOptional()
    @Transform(({ value }) =>
        ['yes', 'true', '1'].includes(value.toLowerCase().trim()),
    )
    isDone?: boolean;
}