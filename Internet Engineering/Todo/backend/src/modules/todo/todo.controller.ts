/* eslint-disable prettier/prettier */
import {
  Controller,
  Get,
  Post,
  Body,
  Param,
  Delete,
  HttpCode,
  HttpStatus,
  ParseIntPipe,
  Put,
  Query,
  UseGuards,
  Patch,
} from '@nestjs/common';
import { TodoService } from './todo.service';
import { CreateTodoDto } from './dto/create-todo.dto';
import { TodoNotFoundException } from 'src/exceptions/todo-notfound-exception';
import { EditTodoDto } from './dto/edit-todo.dto';
import { TodoFilterDto } from './dto/todo-filter.dto';
import { TokenGuard } from '../auth/token.guard';
import { UserID } from '../auth/user.decorator';

@Controller('todo')
export class TodoController {
  constructor(private todoService: TodoService) {}

  @Get()
  listTodo(@Query() filter: TodoFilterDto, @UserID() userId: number) {
    return this.todoService.listTodo(filter, userId);
  }

  @Get(':id')
  @UseGuards(TokenGuard)
  async getTodo(
    @Param('id', ParseIntPipe) id: number,
    @UserID() userId: number,
  ) {
    const todo = await this.todoService.get(id);
    if (!todo || todo.userId !== userId) {
      throw new TodoNotFoundException();
    }
    return todo;
  }

  @Post()
  @UseGuards(TokenGuard)
  addTodo(@Body() data: CreateTodoDto, @UserID() userId: number) {
    return this.todoService.addTodo(data, userId);
  }

  @Delete(':id')
  @UseGuards(TokenGuard)
  @HttpCode(HttpStatus.NO_CONTENT)
  async deleteTodo(@Param('id') id: number, @UserID() userId: number) {
    const todo = await this.todoService.get(id);
    if (!todo || todo.userId !== userId) throw new TodoNotFoundException();

    await this.todoService.deleteTodo(id);
  }
  @Put(':id')
  @UseGuards(TokenGuard)
  async editTodo(
    @Param('id', ParseIntPipe) id: number,
    @Body() data: EditTodoDto,
    @UserID() userId: number,
  ) {
    const todo = await this.todoService.get(id);
    if (!todo || todo.userId !== userId) throw new TodoNotFoundException();

    return this.todoService.editTodo(id, data);
  }

  @Patch(':id')
  updateTodo(@Param('id') id: number, @Body() updateData: { done: boolean }) {
    console.log(`Aktualizacja zadania: ${id}, status done: `, updateData);
    return this.todoService.update(+id, updateData);
  }
}
