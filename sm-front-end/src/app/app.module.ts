import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { LoginComponent } from './auth/login/login.component';
import { ReactiveFormsModule } from '@angular/forms';
import { HomeComponent } from './home/home.component';
import { InteractiveMapComponent } from './home/interactive-map/interactive-map.component';
import { LoginFormComponent } from './auth/login/login-form/login-form.component';
import { RegisterFormComponent } from './auth/login/register-form/register-form.component';
import { DoorListComponent } from './home/door-list/door-list.component';
import { DoorComponent } from './home/door-list/door/door.component';
import { LightBulbListComponent } from './home/light-bulb-list/light-bulb-list.component';
import { LightBulbComponent } from './home/light-bulb-list/light-bulb/light-bulb.component';
import { NavbarComponent } from './home/navbar/navbar.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    HomeComponent,
    InteractiveMapComponent,
    LoginFormComponent,
    RegisterFormComponent,
    DoorListComponent,
    DoorComponent,
    LightBulbListComponent,
    LightBulbComponent,
    NavbarComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    ReactiveFormsModule,
    NgbModule,
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
