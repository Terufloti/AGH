/* eslint-disable prettier/prettier */
import { Injectable } from '@nestjs/common';
import { CreateTodoDto } from './dto/create-todo.dto';
import { PrismaService } from '../prisma/prisma.service';
import { EditTodoDto } from './dto/edit-todo.dto';
import { TodoFilterDto } from './dto/todo-filter.dto';
import { Todo } from '@prisma/client';

@Injectable()
export class TodoService {
  constructor(private readonly prisma: PrismaService) {}

  async listTodo(filter: TodoFilterDto, userId: number) {
    // W tym miejscu musiałem przeciwdziałać obiektowi typu undefined bo uwaga TYPESCRIPT NIE WIE ŻE JUŻ TO ZROBIŁEM W DTO
    const sortBy = filter.sortBy as 'createdAt' | 'title';
    // Bo uwaga, ten fantastyczny język nie ma pojęcia o czymś takim jak IsEnum ewidentnie i jak coś się na etapie kompilacji nie dzieje,
    // to nie może się dziać w ogóle (NA PRZYKŁAD PODCZAS WALIDACJI HTTP REQUESTA)
    const sortOrder = filter.sortOrder as 'asc' | 'desc';

    return this.prisma.todo.findMany({
      where: {
        userId: userId,
        done: filter.isDone,
      },
      orderBy: {
        [sortBy]: sortOrder,
      },
    });
  }
  async addTodo(data: CreateTodoDto, userId: number) {
    return this.prisma.todo.create({
      data: {
        title: data.title,
        content: data.content,
        done: data.done,
        userId: userId,
      },
    });
  }
  editTodo(id: number, data: EditTodoDto) {
    return this.prisma.todo.update({
      where: {
        id,
      },
      data,
    });
  }
  deleteTodo(id: number) {
    return this.prisma.todo.delete({
      where: {
        id,
      },
    });
  }
  async get(id: number) {
    return this.prisma.todo.findUnique({
      where: {
        id,
      },
    });
  }
  async update(id: number, updateData: {done: boolean}): Promise<Todo> {
    return this.prisma.todo.update({
      where: {
        id,
      },
      data: { done: updateData.done },
    });
  }
}
