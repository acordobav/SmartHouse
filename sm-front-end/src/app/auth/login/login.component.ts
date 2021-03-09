import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {
  newUser: boolean = false;

  constructor(
    ) { }

  ngOnInit(): void {

  }

  showRegisterForm(value: boolean) {
    this.newUser = value;
  }



}
