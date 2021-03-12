import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/user.model';

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  private _user: User = null;

  get user(): User { return { ...this._user }; }

  constructor(private http: HttpClient,
    private router: Router) { }

  login(loginData: LoginData) {
    
    /*
    Solicitud HTTP para loguear al usuario
    */

    this._user = new User('Nombre Apellido', 'email@email.com', 'sd2fsd5f4s$%^$d2f^%$#$1sd5fDSKWsd');

    // Almacenamiento en el Local Storage
    sessionStorage.setItem('user', JSON.stringify(this._user));
  }

  autoLogin() {
    const user: User = JSON.parse(sessionStorage.getItem('user'));

    if (!user) {
      return;
    }

    this._user = user;
  }

  isAuthenticated() {
    return !!this._user;
  }

  logout() {
    this._user = null;
    sessionStorage.clear();
    this.router.navigate(['/login']);
  }
}

export interface LoginData {
  email: string,
  password: string,
}
