import { Component, OnInit, Output, EventEmitter } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms';
import { Router } from '@angular/router';
import { AuthService, LoginData } from '../../auth.service';

@Component({
  selector: 'app-login-form',
  templateUrl: './login-form.component.html',
  styleUrls: ['./login-form.component.css']
})
export class LoginFormComponent implements OnInit {
  @Output() newUserEvent = new EventEmitter<void>();
  public errorMessage: string = null;
  public loginForm: FormGroup;

  constructor(
    private router: Router,
    private authService: AuthService,
    ) { }

  ngOnInit(): void {
    this.initForm();
  }

  private initForm() {
    this.loginForm = new FormGroup({
      'email': new FormControl('', [Validators.required, Validators.email]),
      'password': new FormControl('', Validators.required),
    });
  }

  onSubmit() {
    this.errorMessage = null;

    if(this.loginForm.valid) {
      const value = this.loginForm.value;
      this.authenticateUser(value['email'], value['password']);
    
    } else {
      this.errorMessage = "Por favor, ingrese la información solicitada."
    }

  }
  
  private authenticateUser(email: string, password: string) {
    const loginData: LoginData = {email: email, password: password};
    
    //this.authService.login(loginData);
    //.subscribe();

    if(email === "admin@newuser" && password === "password") {
      this.newUserEvent.emit();
    } else {
      this.authService.login(loginData);
      this.redirectToHome();
    }
    
  }
  
  private redirectToHome() {
    this.router.navigate(['/home']);
  }

}
