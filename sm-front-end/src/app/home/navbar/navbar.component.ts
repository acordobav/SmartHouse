import { Component, OnInit } from '@angular/core';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { AuthService } from 'src/app/auth/auth.service';
import { User } from 'src/app/models/user.model';
import { CameraComponent } from '../camera/camera.component';
import { DoorListComponent } from '../door-list/door-list.component';
import { LightBulbListComponent } from '../light-bulb-list/light-bulb-list.component';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.css']
})
export class NavbarComponent implements OnInit {
  public user: User;
  public collapse: boolean = true;

  constructor(private authService: AuthService,
    private modalService: NgbModal) { }

  ngOnInit(): void {
    this.user = this.authService.user;
  }

  onLogout() {
    this.authService.logout();
  }

  onLightBulb() {
    this.modalService.open(LightBulbListComponent, { centered: true, scrollable: true });
  }
  
  onDoor() {
    this.modalService.open(DoorListComponent, { centered: true, scrollable: true });
  }

  onCamera() {
    this.modalService.open(CameraComponent, { size: 'xl', centered: true, scrollable: true });
  }

}
