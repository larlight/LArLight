

int scale(float in) {
  float r = fabs(in);
  float l = sqrt(r);
  float v = l/sqrt(2500) * 127;
  return v;
}

float rgbToHsv(float r, float g, float b){
    r = r/255.;
    g = g/255.; 
    b = b/255.;
    float max = TMath::Max(r, TMath::Max(g, b));
    float min = TMath::Min(r, TMath::Min(g, b));
    float h;
    float v = max;

    float d = max - min;
    float s = max == 0 ? 0 : d / max;

    if(max == min){
        h = 0; // achromatic
    }else{
        switch(max){
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }
    // cout << h << "\t"
    //     << s << "\t"
    //     << v << "\n";

    return h;
}

float hsvToRgb(float h, float s, float v){
    float r, g, b;

    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);            // 0
    float q = v * (1 - f * s);        // 1-f
    float t = v * (1 - (1 - f) * s);  // f

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
    
    cout << (int)(r*255) << "\t"
        << (int)(g*255) << "\t"
        << (int)(b*255) << "\n";
    return rgbToHsv(int(r*255),int(g*255),int(b*255));
    // return [r * 255, g * 255, b * 255];
}

void scan()
{
  float worst = 0;
  for(int x=-2500;x<2500;x+=100){
    float in = (x+2500.)/5000.;
    cout << in << endl;
    float out = hsvToRgb(in,1,1);
    cout << out << endl;
    float y = out*5000. - 2500.;
    worst = TMath::Max(worst, fabs(y-x));
    cout << x << " " << y << " " << y-x << " "  << endl;
  }
  cout << "worst: " << worst << endl;
}