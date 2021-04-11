import { Component, OnInit } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { CameraService } from './camera.service';

@Component({
  selector: 'app-camera',
  templateUrl: './camera.component.html',
  styleUrls: ['./camera.component.css']
})
export class CameraComponent implements OnInit {
  public imagePath: string;
  public location: string = "Ubicación";

  constructor(public activeModal: NgbActiveModal,
    private cameraService: CameraService) { }

  ngOnInit(): void {
    this.takePhoto();
  }

  public takePhoto() {
    this.cameraService.getPhoto().subscribe(
      (response) => {
        this.location = response.location;
        this.imagePath = response.last_photo;
      },
      () => { }
    );
  }

}
