/* eslint-disable prettier/prettier */
import { Global, Module } from '@nestjs/common';
import { TokenService } from './token.service';
import { JwtModule } from '@nestjs/jwt';
import { ConfigService } from '@nestjs/config';

@Global()
@Module({
  imports: [
    JwtModule.registerAsync({
      useFactory: async (config: ConfigService) => ({
        secret: config.get('JWT_KEY'),
      }),
      inject: [ConfigService],
    })
  ],
  providers: [TokenService],
  exports: [TokenService]
})
export class TokenModule {}
