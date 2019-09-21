import { h, Component } from 'preact';
import { Button } from 'preact-fluid';
import { auth, googleAuthProvider } from '../firebase/firebase';

import style from './controls.scss';

interface MyProps {}
interface MyState {
    currentUser: any;
}

const theme = {
    primaryColor: '#72D7C2'
};

export default class Controls extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            if (currentUser) {
                this.setState({ currentUser }, () => console.log('Logged In!'));
            } else {
                this.setState({ currentUser }, () => console.log('Logged Out!'));
            }
        });
    }

    // {user && <p>Signed In As: {name}</p>}
    // <button onClick={() => auth.signOut()}>Sign Out</button>

    render() {
        // const user = this.state.currentUser;
        // const name = user ? user.displayName : 'No one';
        return (
            <div class={style.Stage}>
                <div class={style.Over}>
                    <h3>Sign in to access remote sensor controls</h3>
                    <Button class={style.Button} theme={theme} primary rounded onClick={() => auth.signInWithRedirect(googleAuthProvider)}>
                        Sign In
                    </Button>
                </div>
                <div class={style.Middle} />
                <div class={style.Under}>
                    <h2>Controls Hidden</h2>
                </div>
            </div>
        );
    }
}
