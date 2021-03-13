import { Component, Input, OnInit } from '@angular/core';
import { LightBulb } from 'src/app/models/light-bulb.model';
import { LightBulbService } from '../light-bulb.service';

@Component({
  selector: 'app-light-bulb',
  templateUrl: './light-bulb.component.html',
  styleUrls: ['./light-bulb.component.css']
})
export class LightBulbComponent implements OnInit {
  @Input() lightBulb: LightBulb;

  constructor(private lightBulbService: LightBulbService) { }

  ngOnInit(): void {
  }

  changeState() {
    if(this.lightBulb.state === 1) {
      this.lightBulb.state = 0;
    } else {
      this.lightBulb.state = 1;
    }

    this.lightBulbService.changeState(this.lightBulb.id);
  }

}
