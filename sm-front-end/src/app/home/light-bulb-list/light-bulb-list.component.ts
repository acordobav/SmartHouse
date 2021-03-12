import { Component, OnDestroy, OnInit } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { Subscription } from 'rxjs';
import { LightBulb } from 'src/app/models/light-bulb.model';
import { LightBulbService } from './light-bulb.service';

@Component({
  selector: 'app-light-bulb-list',
  templateUrl: './light-bulb-list.component.html',
  styleUrls: ['./light-bulb-list.component.css']
})
export class LightBulbListComponent implements OnInit, OnDestroy {
  private subLightBulb: Subscription;
  public lightBulbList: LightBulb[] = [];

  constructor(private lightBulbService: LightBulbService,
    public activeModal: NgbActiveModal) { }

  ngOnInit(): void {
    this.subLightBulb = this.lightBulbService.lightBulbs.subscribe(
      (lightBulbList) => {
        this.lightBulbList = lightBulbList;
      }
    );

    this.lightBulbService.fetchData();
  }

  ngOnDestroy() {
    this.subLightBulb.unsubscribe();
  }

  turnOn() {
    this.lightBulbService.turnOn();
  }

  turnOff() {
    this.lightBulbService.turnOff();
  }

}
