import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { User } from '../models/user.model';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  private user: User = null;

  constructor(private http: HttpClient) { }

  login(loginData: LoginData) {
    
    /*
    Solicitud HTTP para loguear al usuario
    */

    this.user = new User('Nombre Apellido', 'email@email.com', 'sd2fsd5f4s$%^$d2f^%$#$1sd5fDSKWsd');
  }

  isAuthenticated() {
    return !!this.user;
  }
}

export interface LoginData {
  email: string,
  password: string,
}
