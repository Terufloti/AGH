/* eslint-disable prettier/prettier */
import { Injectable } from '@nestjs/common';
import { JwtService } from '@nestjs/jwt';

@Injectable()
export class TokenService {
  constructor(private readonly jwtService: JwtService) {}

  createToken(userId: number): string {
    return this.jwtService.sign({ sub: userId }, { expiresIn: '4h' });
  }

  verifyToken(token: string): { sub: number } {
    return this.jwtService.verify(token);
  }
}
