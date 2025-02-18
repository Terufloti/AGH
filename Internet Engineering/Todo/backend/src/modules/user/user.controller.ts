/* eslint-disable */
import { Body, Controller, Get, Post, UseGuards } from '@nestjs/common';
import { UserService } from './user.service';
import { CreateUserDto } from './dto/create-user.dto';
import { plainToInstance } from 'class-transformer';
import { UserDto } from './dto/user.dto';
import { UserID } from '../auth/user.decorator';
import { TokenGuard } from '../auth/token.guard';


@Controller('user')
export class UserController {
    constructor(private readonly userService: UserService) {}

    @Post()
    async create(@Body() createUserDto: CreateUserDto) {
        const user = await this.userService.create(createUserDto);
        return plainToInstance(UserDto, user);
    }

    @Get('/me')
    @UseGuards(TokenGuard)
    async me(@UserID() userId: number) {
        const user = await this.userService.findOne(userId);
        return plainToInstance(UserDto, user);
    }
}
