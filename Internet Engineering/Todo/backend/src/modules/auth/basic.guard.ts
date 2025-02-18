/* eslint-disable */
import {
  CanActivate,
  ExecutionContext,
  Injectable,
  UnauthorizedException,
} from '@nestjs/common';
import { AuthService } from './auth.service';

@Injectable()
export class BasicGuard implements CanActivate {
  constructor(private readonly authService: AuthService) {}

  private decodeAuthHeader(header: string) {
    const b64auth = header.split(' ')[1];
    if (!b64auth)
      throw new UnauthorizedException('Brak danych w nagłówku Auth');
    const decoded = Buffer.from(b64auth, 'base64').toString().split(':');
    console.log('Dekodowane dane: ', decoded);
    if (decoded.length != 2)
      throw new UnauthorizedException('Niepoprawny format Basic Auth');
    return {
      username: decoded[0],
      password: decoded[1],
    };
  }

  async canActivate(context: ExecutionContext): Promise<boolean> {
    const request = context.switchToHttp().getRequest();
    const auth = request.headers['authorization'];
    if (!auth) throw new UnauthorizedException('Brak nagłówka Authorization');

    //TYPESCRIPT MA ZNOWU JAKIŚ PROBLEEEEEEEEEEEEM O UNDEFINED
    const creds = this.decodeAuthHeader(auth);
    if (!creds) throw new UnauthorizedException('Niepoprawne dane logowania');

    const { username, password } = creds;
    if (!username || !password)
      throw new UnauthorizedException('Brak nazwy użytkownika lub hasła');

    const user = await this.authService.verifyUser(username, password);
    if (!user) throw new UnauthorizedException('Bledne dane logowania');
    request.userId = user.id;
    return true;
  }
}
