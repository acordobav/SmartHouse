import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-interactive-map',
  templateUrl: './interactive-map.component.svg',
  styleUrls: ['./interactive-map.component.css']
})
export class InteractiveMapComponent implements OnInit {
  fillColor = 'rgb(255, 0, 0)';

  constructor() { }

  ngOnInit(): void {
  }

  onClick() {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    this.fillColor = `rgb(${r}, ${g}, ${b})`;
  }


  changeColor() {
    const r = Math.floor(Math.random() * 256);
    const g = Math.floor(Math.random() * 256);
    const b = Math.floor(Math.random() * 256);
    this.fillColor = `rgb(${r}, ${g}, ${b})`;
  }

}
