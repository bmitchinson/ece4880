import { h } from 'preact';
import style from './header.scss';

const iconImageLink = 'https://res.cloudinary.com/dheqbiqti/image/upload/r_max/v1568422477/Projects/TempCtrl/icon.png';

const Header = () => (
    <header class={style.header}>
        <div>
            <img src={iconImageLink} alt="icon" />
        </div>
        <h1>Temp Ctrl</h1>
    </header>
);

export default Header;
