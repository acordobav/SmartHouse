import { Component, OnInit, Output, EventEmitter } from '@angular/core';
import { FormControl, FormGroup, Validators } from '@angular/forms';

@Component({
  selector: 'app-register-form',
  templateUrl: './register-form.component.html',
  styleUrls: ['./register-form.component.css']
})
export class RegisterFormComponent implements OnInit {
  @Output() finishedEvent = new EventEmitter<void>();
  public registerForm: FormGroup;
  public errorMessage: string = null;
  public wrongPasswords: boolean = false;

  constructor() { }

  ngOnInit(): void {
    this.initForm();
  }

  private initForm() {
    this.registerForm = new FormGroup({
      'name': new FormControl('', Validators.required),
      'email': new FormControl('', [Validators.required, Validators.email]),
      'password1': new FormControl('', [Validators.required, Validators.minLength(8)]),
      'password2': new FormControl('', [Validators.required, Validators.minLength(8)]),
    });
  }

  onSubmit() {
    this.errorMessage = null;

    const value = this.registerForm.value;
    
    // Checks if the form is valid
    if (!this.registerForm.valid) {
      this.errorMessage = 'Por favor, ingrese la información solicitada.'
      return;
    }

    // Checks if the password are equal
    if (!this.checkPasswords(value['password1'], value['password2'])) 
      return;


    this.emitFinishedEvent();
      
      
  }

  /**
   * Metodo que verifica que las dos passwords sean iguales
   * @param password1 
   * @param password2 
   */
  private checkPasswords(password1: string, password2: string) {
    this.wrongPasswords = false;
    // Se verifica que las password sean iguales
    if (password1 === password2) {
      return true;
    }
    this.errorMessage = 'Las contraseñas no coinciden.'
    this.wrongPasswords = true;
    return false;
  }

  private emitFinishedEvent() {
    this.finishedEvent.emit();
  }
}
