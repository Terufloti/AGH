/* eslint-disable prettier/prettier */
import { Module } from '@nestjs/common';
import { AuthService } from './auth.service';
import { PrismaModule } from '../prisma/prisma.module';
import { AuthController } from './auth.controller';

@Module({
  providers: [AuthService],
  imports: [PrismaModule],
  controllers: [AuthController],
})
export class AuthModule {}
