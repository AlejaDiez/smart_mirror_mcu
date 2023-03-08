const brightnessTabButton = document.querySelector('#brightness_tab_button');
const brightnessSpan = brightnessTabButton.querySelector('span');
const brightnessSection = document.querySelector('#brightness_section');
const brightnessPicker = document.querySelector('#brightness_picker');
const colorTabButton = document.querySelector('#color_tab_button');
const colorSection = document.querySelector('#color_section');
const colorPicker = iro.ColorPicker("#color_picker", {
  width: document.body.offsetWidth < document.body.offsetHeight ? document.body.offsetWidth * 0.64 : document.body.offsetHeight * 0.4,
  color: colorTabButton.style.getPropertyValue('--color').match('#[0-9A-F]{6}') ? colorTabButton.style.getPropertyValue('--color') : "#FFFFFF",
  layout: [{ component: iro.ui.Wheel }]
});
const powerButton = document.querySelector('#power_button');

window.onresize = () => {
  colorPicker.resize(document.body.offsetWidth < document.body.offsetHeight ? document.body.offsetWidth * 0.64 : document.body.offsetHeight * 0.4);
};

brightnessTabButton.addEventListener('click', () => {
  brightnessTabButton.setAttribute('active', "");
  brightnessSection.setAttribute('active', "");
  colorTabButton.removeAttribute('active');
  colorSection.removeAttribute('active');
});
brightnessPicker.addEventListener('input', event => {
  const value = event.target.value;

  brightnessTabButton.style.setProperty('--percentage', value);
  brightnessSpan.innerHTML = value;
  brightnessPicker.style.setProperty('--value', `${value}%`);
});
brightnessPicker.addEventListener('change', event => {
  const value = event.target.value;

  fetch(`/brightness/${value}`, { method: 'POST' });
});
colorTabButton.addEventListener('click', () => {
  brightnessTabButton.removeAttribute('active');
  brightnessSection.removeAttribute('active');
  colorTabButton.setAttribute('active', "");
  colorSection.setAttribute('active', "");
});
colorPicker.on('color:change', value => colorTabButton.style.setProperty('--color', value.hexString));
colorPicker.on('input:end', value => {
  const hex = value.hexString.replace('#', "");

  fetch(`/color/${hex}`, { method: 'POST' });
});
powerButton.addEventListener('click', () => {
  fetch(powerButton.hasAttribute('active') ? '/state/off' : '/state/on', { method: 'POST' }).then(response => {
    if (response.status === 200) {
      powerButton.toggleAttribute('active');
    }
  });
});