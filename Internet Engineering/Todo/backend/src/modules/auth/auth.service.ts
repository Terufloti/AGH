/* eslint-disable */
import { Injectable } from '@nestjs/common';
import { User } from '@prisma/client';
import * as argon2 from 'argon2';
import { PrismaService } from '../prisma/prisma.service';

@Injectable()
export class AuthService {
    constructor(private readonly prisma: PrismaService) {}

    async verifyUser(username: string, password: string): Promise<User | null> {
        console.log("Sprawdzanie uzytkownika: ",username);
        const user = await this.prisma.user.findUnique({
            where: { email: username },
        });
        if(!user) {
            console.log("Uzytkownik ", username," nie znaleziony w bazie.")
            return null;
        }
        const isValid = await argon2.verify(user.password, password);

        if(!isValid) {
            console.log("Haslo dla uzytkownika ", username," niepoprawne.")
            return null;
        }
        return user;
    }
}
