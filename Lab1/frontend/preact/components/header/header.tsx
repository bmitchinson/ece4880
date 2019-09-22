import { h, Component } from 'preact';
import { Button } from 'preact-fluid';
import { auth } from '../firebase/firebase';

import style from './header.scss';

const iconImageLink = 'https://res.cloudinary.com/dheqbiqti/image/upload/r_max/v1568422477/Projects/TempCtrl/icon.png';

const theme = {
    borderColor: '#f0f7ee',
    linkColor: '#f0f7ee'
};

interface MyProps {}
interface MyState {
    currentUser: any;
}

export default class Header extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            if (currentUser) {
                this.setState({ currentUser });
            } else {
                this.setState({ currentUser });
            }
        });
    }

    render() {
        const user = this.state.currentUser;
        return (
            <div>
                <header class={style.header}>
                    <div>
                        <img src={iconImageLink} alt="icon" />
                    </div>
                    <h1>Temp Ctrl</h1>
                    {user && (
                        <div class={style.ButtonContainer}>
                            <Button theme={theme} onClick={() => auth.signOut()}>
                                Logout
                            </Button>
                        </div>
                    )}
                </header>
                <div class={style.headerpad} />
            </div>
        );
    }
}
