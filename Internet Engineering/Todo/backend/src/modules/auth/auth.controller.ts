/* eslint-disable prettier/prettier */
import { Controller, HttpStatus, Post, Res, UseGuards, HttpCode } from '@nestjs/common';
import { BasicGuard } from './basic.guard';
import { UserID } from './user.decorator';
import { TokenService } from '../token/token.service';
import { Response } from 'express';


@Controller('auth')
export class AuthController {
  constructor(private readonly tokenService: TokenService) {}

  @Post('login')
  @UseGuards(BasicGuard)
  @HttpCode(HttpStatus.OK)
  login(@UserID() userId: number, @Res({ passthrough: true }) res: Response) {
    const token: string = this.tokenService.createToken(userId);
    res.cookie('access-token', token, {
        httpOnly: true,
        expires: new Date(Date.now() + 4 * 60 * 60 * 1000),
    });
    res.cookie('is-logged', true, {
        expires: new Date(Date.now() + 24 * 60 * 60 * 1000),
    });
    return true;
  }

  @Post('logout')
  logout(@Res({ passthrough: true }) res: Response) {
    res.clearCookie('access-token');
    res.clearCookie('is-logged');
  }
}
